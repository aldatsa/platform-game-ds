#ifndef PLAYER_H
#define PLAYER_H

class Player {
	
	private:
		u8* frame_gfx;
		int anim_frame;
	
	public:
		int x;
		int y;
		
		int vx;
		int vy;
		
		int state;
		
		u16* sprite_gfx_mem;
		
		Player(u8* gfx, int x, int y, int vx, int vy);
		
		void tileCollisionDetection(u16* level);
		
		void calculateNewPosition();
		
		void animate();
		
};

#endif
