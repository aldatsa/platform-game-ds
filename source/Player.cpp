#include <stdio.h>

#include <nds.h>

#include "Player.hpp"

#define FRAMES_PER_ANIMATION 3

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

#define MARIO_WIDTH 32
#define MARIO_HEIGHT 32

#define MARIO_WIDTH_TILES 3
#define MARIO_HEIGHT_TILES 3

#define WORLD_WIDTH_TILES 128
#define WORLD_HEIGHT_TILES 24

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

void Player::tileCollisionDetection(u16* level) {
	
	// Tiles that the player can't pass through. 
	int solid_tiles[4] = {12, 13, 14, 15};
	
	int tile_x = this->x / TILE_WIDTH;
	
	int previous_tile_y = (this->previous_y + MARIO_HEIGHT - 1) / TILE_HEIGHT;
	int current_tile_y = (this->y + MARIO_HEIGHT - 1) / TILE_HEIGHT;
	
	int collision = false;
	
	// For each tile between the previous one and the current one, test if there is a collision.
	for (int tile_y = previous_tile_y; tile_y <= current_tile_y; tile_y++) {
		
		int tile_index = tile_y * WORLD_WIDTH_TILES + tile_x;	
		
		for (int i = 0; i < sizeof(solid_tiles)/sizeof(int); i++) {
			
			if (level[tile_index] == solid_tiles[i]) {
				collision = true;
			}
			
		}
	
		if (collision == true) {
			
			// Put the player on top of the first solid tile.
			this->y = (tile_y - MARIO_HEIGHT_TILES - 1) * TILE_HEIGHT;
			
			// Stop the falling movement of the player.
			this->vy = 0;
			
			break;
			
		}
		
	}
	
}

void Player::calculateNewPosition() {
	
	this->previous_x = this->x;
	this->previous_y = this->y;
	
	this->x = this->x + this->vx;
	this->y = this->y + this->vy;
	
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
