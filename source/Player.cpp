#include <stdio.h>

#include <nds.h>

#include "Player.hpp"

#define FRAMES_PER_ANIMATION 3

#define GRAVITY 1

#define WORLD_WIDTH 1024
#define WORLD_HEIGHT 192

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

#define MARIO_WIDTH 32
#define MARIO_HEIGHT 32

#define MARIO_WIDTH_TILES 3
#define MARIO_HEIGHT_TILES 3

#define WORLD_WIDTH_TILES 128
#define WORLD_HEIGHT_TILES 24

//---------------------------------------------------------------------
// The state of the sprite (which way it is walking)
//---------------------------------------------------------------------
#define W_UP 0
#define W_RIGHT 1
#define W_DOWN 2
#define W_LEFT 3

Player::Player(u8* gfx, int x = 0, int y = 0, int vx = 0, int vy = 0) {
	
	this->x = x;
	this->y = y;
	
	this->vx = vx;
	this->vy = vy;
	
	this->previous_x = x;
	this->previous_y = y;
	
	this->state = 0;
	this->anim_frame = 0;
	
	// Allocate room for one frame.
	this->sprite_gfx_mem = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	
	// Set the frame gfx pointer
	this->frame_gfx = (u8*)gfx;
	
}

void Player::tileCollisionDetectionX(u16* level) {
	
	// Tiles that the player can't pass through. 
	int solid_tiles[] = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
	
	int collision = false;
	
	int tile_y_top = this->previous_y / TILE_HEIGHT;
	
	int previous_tile_x = this->previous_x / TILE_WIDTH;
	int current_tile_x = this->x / TILE_WIDTH;
	
	if (this->vx > 0) {
		
		// For each tile in front of the player.
		for (int tile_y = tile_y_top; tile_y <= tile_y_top + MARIO_HEIGHT_TILES; tile_y++) {
			
			// For each tile between the previous one and the current one, test if there is a collision.
			for (int tile_x = previous_tile_x; tile_x <= current_tile_x; tile_x++) {
				
				int tile_index = tile_y * WORLD_WIDTH_TILES + tile_x + MARIO_WIDTH_TILES;	
				
				for (int i = 0; i < sizeof(solid_tiles) / sizeof(int); i++) {
					
					if (level[tile_index] == solid_tiles[i]) {
						
						collision = true;
						
						printf("%d\n", tile_x);
						printf("%d\n", this->x);
						
						// Put the player in front of the first solid tile.
						this->x = tile_x * TILE_WIDTH;
						
						printf("%d\n", this->x);
						
						// Stop the forward movement of the player.
						this->vx = 0;
						
						break;
						
					}
					
				}
				
				if (collision == true) {
					
					break;
					
				}
			
			}
			
			if (collision == true) {
				
				break;
				
			}
			
		}
			
	} else if (this->vx < 0) {
		
		// For each tile in front of the player.
		for (int tile_y = tile_y_top; tile_y <= tile_y_top + MARIO_HEIGHT_TILES; tile_y++) {
			
			// For each tile between the previous one and the current one, test if there is a collision.
			for (int tile_x = previous_tile_x; tile_x >= current_tile_x; tile_x--) {
				
				// Without the plus one the player stops too far from the obstacle.
				int tile_index = tile_y * WORLD_WIDTH_TILES + tile_x + 1;
				
				for (int i = 0; i < sizeof(solid_tiles) / sizeof(int); i++) {
					
					if (level[tile_index] == solid_tiles[i]) {
						
						collision = true;
						
						printf("%d\n", previous_tile_x);
						printf("%d\n", current_tile_x);
						
						printf("%d\n", tile_x);
						printf("%d\n", this->x);
						
						// Put the player in front of the first solid tile.
						this->x = (tile_x + 1) * TILE_WIDTH;
						
						printf("%d\n", this->x);
						
						this->vx = 0;
						
						break;
						
					}
					
				}
				
				if (collision == true) {
					
					break;
					
				}
				
			}
			
			if (collision == true) {
				
				break;
				
			}
		
		}
	
	}
	
}

void Player::tileCollisionDetectionY(u16* level) {
	
	// Tiles that the player can't pass through. 
	int solid_tiles[] = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
	
	int collision = false;
	
	// I don't understand why, but without the plus one the player jumps over the obstacles of on its left.
	int tile_x_left = this->x / TILE_WIDTH + 1;
	
	if (this->vy > 0) {
		
		int previous_tile_y = (this->previous_y + MARIO_HEIGHT - 1) / TILE_HEIGHT;
		int current_tile_y = (this->y + MARIO_HEIGHT - 1) / TILE_HEIGHT;
		
		// For each tile under the player.
		for (int tile_x = tile_x_left; tile_x < tile_x_left + MARIO_WIDTH_TILES; tile_x++) {
			
			// For each tile between the previous one and the current one, test if there is a collision.
			for (int tile_y = previous_tile_y; tile_y <= current_tile_y; tile_y++) {
				
				int tile_index = tile_y * WORLD_WIDTH_TILES + tile_x;	
				
				for (int i = 0; i < sizeof(solid_tiles)/sizeof(int); i++) {
					
					if (level[tile_index] == solid_tiles[i]) {
						
						collision = true;
						
						// Put the player on top of the first solid tile.
						this->y = (tile_y - MARIO_HEIGHT_TILES - 1) * TILE_HEIGHT;
						
						// Stop the falling movement of the player.
						this->vy = 0;
						
						break;
						
					}
					
				}
				
				if (collision == true) {
					
					break;
					
				}
				
			}
			
			if (collision == true) {
				
				break;
				
			}
			
		}
		
	} else {
		
		int previous_tile_y = (this->previous_y - 1) / TILE_HEIGHT;
		int current_tile_y = (this->y - 1) / TILE_HEIGHT;
		
		// For each tile over the player.
		for (int tile_x = tile_x_left; tile_x < tile_x_left + MARIO_WIDTH_TILES; tile_x++) {
			
			for (int tile_y = previous_tile_y; tile_y >= current_tile_y; tile_y--) {
				
				int tile_index = tile_y * WORLD_WIDTH_TILES + tile_x;	
				
				for (int i = 0; i < sizeof(solid_tiles)/sizeof(int); i++) {
					
					if (level[tile_index] == solid_tiles[i]) {
						
						collision = true;
						
						this->y = tile_y * TILE_HEIGHT;
						
						this->vy = 0;
						
						break;
						
					}
					
				}
				
				if (collision == true) {
					
					break;
					
				}
				
			}
			
			if (collision == true) {
				
				break;
				
			}
		
		}
	
	}
	
}

void Player::calculateNewPosition() {
	
	this->previous_x = this->x;
	this->previous_y = this->y;
	
	this->x = this->x + this->vx;
	this->y = this->y + this->vy;
	
}

void Player::calculateNewVelocity() {
	
	int keys_held = 0;
	int keys_down = 0;

	scanKeys();

	keys_held = keysHeld();
	keys_down = keysDown();

	if((keys_held & KEY_LEFT) && (this->x > 0)) {
		
		if (keys_held & KEY_B) {
			
			this->vx = -4;
			
		} else {
			
			this->vx = -1;
			
		}
		
		this->state = W_LEFT;

	} else if ((keys_held & KEY_RIGHT) && (this->x < WORLD_WIDTH - MARIO_WIDTH) ) {

		if (keys_held & KEY_B) {
			
			this->vx = 4;
			
		} else {
			
			this->vx = 1;
			
		}

		this->state = W_RIGHT;

	} else {
		
		this->vx = 0;
		
	}

	if (keys_down & KEY_A) {

		// Jump Mario! Jump!
		this->vy = -10;
	}
	
	// Apply gravity.
	this->vy = this->vy + GRAVITY;
	
}

// Animating the player requires us to copy in a new frame of data each time.
void Player::animate() {
	
	if (this->vx != 0) {
	
		this->anim_frame++;

		if(this->anim_frame >= FRAMES_PER_ANIMATION) this->anim_frame = 0;
	
	}
	
	int frame = this->anim_frame + this->state * FRAMES_PER_ANIMATION;

	u8* offset = this->frame_gfx + frame * 32*32;

	dmaCopy(offset, this->sprite_gfx_mem, 32*32);
	
}

void Player::update() {
	
	if (this->x < SCREEN_WIDTH / 2) {

		oamSet(&oamMain, 0, this->x, this->y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
			this->sprite_gfx_mem, -1, false, false, false, false, false);

	} else if (this->x > WORLD_WIDTH - SCREEN_WIDTH / 2) {

		oamSet(&oamMain, 0, this->x - WORLD_WIDTH - SCREEN_WIDTH, this->y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
			this->sprite_gfx_mem, -1, false, false, false, false, false);

	} else {

		oamSet(&oamMain, 0, SCREEN_WIDTH / 2, this->y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
			this->sprite_gfx_mem, -1, false, false, false, false, false);

	}
	
	oamUpdate(&oamMain);
	
}
