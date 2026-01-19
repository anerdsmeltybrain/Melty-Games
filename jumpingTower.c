#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
	bool unlit;
};

struct Level {
	int numBlocks;
	int startNum;
	int endNum;
	int currentBlock;
	int litBlocks;
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
	float jumpTimer;
	int playerScore;
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

void genLadder(struct Level *);
void genFloor(struct Level *);

int main() {
	int screenWidth = 640;
	int screenHeight = 480;
	srand(time(NULL));
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
		level.update(&level);
		camera.target = (Vector2){player.x, player.y};

		BeginDrawing();
		ClearBackground(BLACK);
		static bool debug = true;
		if(debug) {
			DrawText(TextFormat("P X: %d", player.x), 0, 0, 16, WHITE);
			DrawText(TextFormat("P Y: %d", player.y), 0, 16, 16, WHITE);
			DrawText(TextFormat("P can Jump: %d", player.canJump), 0, 32, 16, WHITE);
			DrawText(TextFormat("P jump Buffer: %f", player.jumpBuffer), 0, 48, 16, WHITE);
			DrawText(TextFormat("P jump Counter: %d", player.jumpCounter), 0, 64, 16, WHITE);
			DrawText(TextFormat("P jump Speed: %f", player.jumpSpeed), 0, 80, 16, WHITE);
			DrawText(TextFormat("P is Grounded: %d", player.isGrounded), 0, 96, 16, WHITE);
			DrawText(TextFormat("P fallSpeed: %f", player.fallSpeed), 0, 112, 16, WHITE);
			DrawText(TextFormat("P Score: %d", player.playerScore), 0, 128, 16, WHITE);
		}

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
			b->color = LIME;
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
	p->playerScore = 0;

}

void updatePlayer( struct Player * p, struct Level * l ) {


	if(IsKeyDown(KEY_A)) {
		p->x -= p->runSpeed;
	} else if (IsKeyDown(KEY_D)) {
		p->x += p->runSpeed;
	}

	if(IsKeyDown(KEY_W) && p->isGrounded == true && p->canJump == true) {
		p->jumpBuffer += 0.4;
		if(p->jumpBuffer > 5) {
			p->jumpBuffer = 5;
			p->jumpSpeed += p->jumpBuffer;
		}
	}

	if (IsKeyReleased(KEY_W)) {
		if(p->jumpSpeed > 5) {
			p->jumpSpeed = 5;
		}
		p->isGrounded = false;
		p->canJump = false;
	}

	if(p->isGrounded == false) {
		p->fallSpeed += 0.05;
		p->y += GRAVITY + p->fallSpeed;

		if(p->jumpSpeed < p->jumpBuffer) {
			p->jumpSpeed += p->jumpSpeed;
		} else {
			p->jumpCounter++;
		}

		if(p->jumpCounter < 50 && p->canJump == false) {
			p->y -= p->jumpSpeed + p->fallSpeed;
			// p->y -= p->fallSpeed;
		} 
	}

	for(int i = 0; i < l->endNum; i++) {
		if(CheckCollisionRecs((Rectangle){p->x, p->y, p->width, p->height}, l->blocks[i].rect) && l->blocks[i].bt == GRASS) {
			if(l->blocks[i].unlit == false) {
				l->blocks[i].unlit = true;
				l->blocks[i].color = GREEN;
				l->litBlocks++;
				p->playerScore++;
			}
			if (p->y + p->height + p->fallSpeed >= l->blocks[i].rect.y) {
				p->y -= GRAVITY + 1;
			}
			p->isGrounded = true;
			p->jumpCounter = 0;
			p->jumpSpeed = 0.2;
			p->jumpBuffer = 1.0;
			p->fallSpeed = 0.0;
			p->canJump = true;
		} else {
			p->isGrounded = false;
		}

		if(CheckCollisionRecs((Rectangle){p->x, p->y, p->width, p->height}, l->blocks[i].rect) && l->blocks[i].bt == LADDER) {
			p->fallSpeed = 0;
			p->canJump = true;
			p->isGrounded = true;
			if(l->blocks[i].unlit == false) {
				l->blocks[i].unlit = true;
				l->blocks[i].color = ORANGE;
				l->litBlocks++;
				p->playerScore++;
			}

			if(IsKeyDown(KEY_SPACE)) {
				p->y -= GRAVITY + 1;
			}
		} else {
			p->isGrounded = false;
		}
	}
}

void drawPlayer ( struct Player * p ) {

	DrawRectangle(p->x, p->y, p->width, p->height, p->color);

}

void initLevel( struct Level * l) {
	
	l->numBlocks = 10;
	l->startNum = 0;
	l->endNum = l->numBlocks - 1;
	l->currentBlock = 0;
	l->litBlocks = 0;

	l->blocks = malloc(l->numBlocks * sizeof(struct Block));

	for(l->currentBlock = l->startNum; l->currentBlock < l->endNum; l->currentBlock++) {
		l->blocks[l->currentBlock].rect = (Rectangle){l->currentBlock * 32, 256, 32, 32};
		l->blocks[l->currentBlock].bt = GRASS;
		chooseBlockColor(&l->blocks[l->currentBlock]);
		l->blocks[l->currentBlock].unlit = false;
	}
}

void updateLevel ( struct Level * l ) {

	static bool onLadder = true;
	static bool onFloor = false;

	
	if(l->litBlocks >= l->numBlocks - 1) {

		// for(l->currentBlock = l->startNum; l->currentBlock < l->endNum; l->currentBlock++) {
		// 	l->blocks[l->currentBlock].rect = (Rectangle){l->currentBlock * 32, 256, 32, 32};
		// 	l->blocks[l->currentBlock].bt = STONE;
		// 	chooseBlockColor(&l->blocks[l->currentBlock]);
		// 	l->blocks[l->currentBlock].unlit = false;
		// }

		if(onLadder) {
			
			onLadder = false;
			l->numBlocks += 10;
			l->startNum = l->currentBlock;
			l->endNum = l->numBlocks - 1;

			l->blocks = realloc(l->blocks, l->numBlocks * sizeof(struct Block));
			genLadder(l);
		} else {

			int random = (rand() % 5);

			onLadder = true;
			l->numBlocks += 10 + random;
			l->startNum = l->currentBlock;
			l->endNum = l->numBlocks - 1;

			l->blocks = realloc(l->blocks, l->numBlocks * sizeof(struct Block));
			genFloor(l);
		}
	}

}

void genLadder ( struct Level * l ) {
		for(l->currentBlock = l->startNum; l->currentBlock < l->endNum; l->currentBlock++) {
			l->blocks[l->currentBlock].rect = (Rectangle){l->blocks[l->currentBlock - 1].rect.x, l->blocks[l->currentBlock - 1].rect.y - 32, 32, 32};
			l->blocks[l->currentBlock].bt = LADDER;
			chooseBlockColor(&l->blocks[l->currentBlock]);
			l->blocks[l->currentBlock].unlit = false;
		}
}

void genFloor ( struct Level * l ) {
		for(l->currentBlock = l->startNum; l->currentBlock < l->endNum; l->currentBlock++) {
			l->blocks[l->currentBlock].rect = (Rectangle){l->blocks[l->currentBlock - 1].rect.x + 32, l->blocks[l->currentBlock - 1].rect.y, 32, 32};
			l->blocks[l->currentBlock].bt = GRASS;
			chooseBlockColor(&l->blocks[l->currentBlock]);
			l->blocks[l->currentBlock].unlit = false;
		}
}

void drawLevel( struct Level * l ) {
	for(int i = 0; i < l->endNum; i++) {
		DrawRectangleRec(l->blocks[i].rect, l->blocks[i].color);
	}
}
