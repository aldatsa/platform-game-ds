#include <nds.h>

#include "Camera.hpp"

#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define SCREEN_HEIGHT_TILES 24
#define SCREEN_WIDTH_TILES 32
#define WORLD_WIDTH_TILES 128
#define WORLD_HEIGHT_TILES 24

Camera::Camera(u16* level) {
	
	this->level = level;
	
	//get the address of the tile and map blocks
	this->mapMemory = (u16*)BG_MAP_RAM(0);
	
}

void Camera::update() {
	
	// Update the map in map memory
	for (int j = 0; j < SCREEN_HEIGHT_TILES; j++) {
		for(int i = 0; i < SCREEN_WIDTH_TILES; i++) {
			this->mapMemory[i + SCREEN_WIDTH_TILES * j] = this->level[this->x / TILE_WIDTH + i + j * WORLD_WIDTH_TILES];
		}
	}
}
