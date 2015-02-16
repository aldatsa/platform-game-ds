// Big thanks to Dev-Scene for providing their Tile Mapping tutorial.
// NDS/Tutorials Day 4 - Dev-Scene http://www.dev-scene.com/NDS/Tutorials_Day_4
//  About 3/4 of this file is from their original example.
//    - Cthulhu32 01/29/2008
// Taken from (made a pair of changes to make it work):
// http://cthulhu32.kraln.com/tutorials/dynamicscroll.html
#include <nds.h>

//create a red 8x8 tile called redTile
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

//create a green 8x8 tile called greenTile
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

// create a blue 8x8 tile called blueTile
u8 blueTile[64] =
{
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3
};

// Map Data is a (10 by 4) Array full of 16x16 blocks
u16 mapData[40] =
{
	3,3,1,3,2,2,2,2,2,2,
	3,3,1,1,3,2,2,2,2,2,
	3,3,3,1,1,1,3,2,2,2,
	3,3,3,3,1,1,1,1,1,1
};

void updateMapMemory(int x, int y)
{
	int i,ix,iy;
	u16* mapMemory = (u16*)BG_MAP_RAM(0);
	for ( i = 0; i < 48; i++) // our screen is 8x6 = 48
	{
		ix = i % 8;
		iy = i / 8;
		mapMemory[((iy)*32) + (ix)] = mapData[((y+iy)/2)*10 + ((x+ix)/2)%10]; //The Nintendo DS has a 32x32 tiled screen as defined earlier (BG_32x32)
	}
}

//---------------------------------------------------------------------------------
int main(void) {
	//---------------------------------------------------------------------------------
	int x,y; // Our current x & y positions in the map data
	int pressed; // Used for key input

	x = y = 0; // Set both x & y to 0 initially

	// powerON renamed to powerOn
	powerOn(POWER_ALL_2D);
	lcdMainOnBottom();

	// initialise the irq dispatcher
	irqInit();
	// a vblank interrupt is needed to use swiWaitForVBlank()
	// since the dispatcher handles the flags no handler is required
	irqEnable(IRQ_VBLANK);

	//set video mode and map vram to the background
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	//get the address of the tile and map blocks
	u8* tileMemory = (u8*)BG_TILE_RAM(1);
	//u16* mapMemory = (u16*)BG_MAP_RAM(0);

	//tell the DS where we are putting everything and set 256 color mode and that we are using a 32 by 32 tile map.
	// BG0_CR renamed to REG_BG0CNT
	// http://devkitpro.org/viewtopic.php?f=6&t=1512
	REG_BG0CNT = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	//load our palette
	BG_PALETTE[1] = RGB15(31,0,0); // red color
	BG_PALETTE[2] = RGB15(0,31,0); // green color
	BG_PALETTE[3] = RGB15(0,0,31); // blue color

	DC_FlushAll();

	//copy the tiles into tile memory one after the other
	dmaCopy(redTile, tileMemory + 64, 64);
	dmaCopy(greenTile, tileMemory + 128, 64);
	dmaCopy(blueTile, tileMemory + 192, 64);

	// Init the screen!
	updateMapMemory(x,y);

	while(1)
	{
		// read the button states
		scanKeys();

		pressed = keysDown();	// buttons pressed this loop

		if( pressed & KEY_LEFT  ) // we want to move left & our x is not 0 (goes to negatives otherwise)
		{
			if ( x > 0 ){
				x--;
				updateMapMemory(x,y);
			}
		}
		if( pressed & KEY_RIGHT )
		{
			if ( x < 12 ){
				x++;
				updateMapMemory(x,y);
			}
		}
		if ( pressed & KEY_UP)
		{
			if ( y > 0 ){
				y--;
				updateMapMemory(x,y);
			}
		}
		if ( pressed & KEY_DOWN )
		{
			if ( y < 2 ){
				y++;
				updateMapMemory(x,y);
			}
		}
		swiWaitForVBlank();
	}

	return 0;
}
