#include <nds.h>

//create a tile called redTile
u8 redTile[64] =
{
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1
};

//create a tile called greenTile
u8 greenTile[64] =
{
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2
};


//---------------------------------------------------------------------------------
int main(void) {
	//---------------------------------------------------------------------------------
	int i;

	//set video mode and map vram to the background
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	//get the address of the tile and map blocks
	u8* tileMemory = (u8*)BG_TILE_RAM(1);
	u16* mapMemory = (u16*)BG_MAP_RAM(0);

	//tell the DS where we are putting everything and set 256 color mode and that we are using a 32 by 32 tile map.
	REG_BG0CNT = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	//load our palette
	BG_PALETTE[1] = RGB15(31,0,0);
	BG_PALETTE[2] = RGB15(0,31,0);


	//copy the tiles into tile memory one after the other
	swiCopy(redTile, tileMemory, 32);
	swiCopy(greenTile, tileMemory + 64, 32);

	//create a map in map memory
	for(i = 0; i < 32 * 32; i++) {
		mapMemory[i] = i & 1;
	}

	while(1) {
		swiWaitForVBlank();
	}
	
	return 0;
}